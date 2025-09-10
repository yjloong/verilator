#!/usr/bin/env python3
import os
import errno
import sys
from Verilog_VCD import parse_vcd
import fcntl
from multiprocessing import Process, Queue

global q
q = Queue()

siglists = ['TOP.top.out_wide[69:0]']

num_processes = 4

FIFO_NAME = sys.argv[1]

def process_handler(queue: Queue):
    print(siglists)
    pass

def main():
    # 创建 FIFO (如果已存在则忽略错误)
    try:
        if os.path.exists(FIFO_NAME):
            os.unlink(FIFO_NAME)
        os.mkfifo(FIFO_NAME, 0o666)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise

    print("等待写入进程打开FIFO...")
    
    try:
        processors = []
        for i in range(num_processes):
            p = Process(target=process_handler, args=(q,))
            p.start()
            processors.append(p)
        #data = parse_vcd(sys.argv[1], siglist=['TOP.top.out_wide[69:0]'], use_stdout=1)
        #print(data)
        #return
        
        with open(FIFO_NAME, 'r') as fifo, open('output.txt', 'w') as output:
            fcntl.fcntl(fifo, fcntl.F_SETPIPE_SZ, 0x100000)
            while True:
                data = fifo.read()
                if data:
                    q.put_nowait(data)
                else:
                    break
            
        
        #print(data)
    except KeyboardInterrupt:
        print("\n接收到中断信号，退出...")
    finally:
        # 清理FIFO文件
        try:
            os.unlink(FIFO_NAME)
        except OSError:
            pass

if __name__ == '__main__':
    main()