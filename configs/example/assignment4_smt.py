import m5
from m5.objects import *
from caches import *

import argparse

# Argument parsing for binary and cache sizes
parser = argparse.ArgumentParser(description='A simple system with 2-level cache and SMT support.')
parser.add_argument("binary", default="tests/test-progs/workloads/bin/intVec", nargs="?", type=str,
                    help="Path to the binary to execute for thread 1.")
parser.add_argument("--l1i_size", default="16kB",
                    help="L1 instruction cache size. Default: 16kB.")
parser.add_argument("--l1d_size", default="64kB",
                    help="L1 data cache size. Default: 64kB.")
parser.add_argument("--l2_size", default="256kB",
                    help="L2 cache size. Default: 256kB.")

options = parser.parse_args()

# Create the system object (the parent of all objects)
system = System()

# Set up the system clock
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Specify memory system and adjust the memory range to match DRAM capacity
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('8GB')]  # Match this with the DRAM capacity of 8GB

# Configure two X86O3CPUs with separate workloads
cpu_num = 4
system.cpu = [O3CPU(cpu_id=i) for i in range(cpu_num)]
system.membus = SystemXBar()
system.system_port = system.membus.cpu_side_ports

# Create and configure DRAM controller
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

# Initialize SEWorkload for compatibility with each binary
system.workload = SEWorkload.init_compatible(options.binary)
# Create processes
process = Process(executable=options.binary, cmd = [options.binary, cpu_num])

# Assign workloads to each CPU and enable threading
for i in range(cpu_num):
    system.cpu[i].workload = process
    system.cpu[i].createThreads()

# Configure interrupt controllers for each CPU
for i in range(cpu_num):
    system.cpu[i].createInterruptController()
    system.cpu[i].interrupts[0].pio = system.membus.mem_side_ports
    system.cpu[i].interrupts[0].int_requestor = system.membus.cpu_side_ports
    system.cpu[i].interrupts[0].int_responder = system.membus.mem_side_ports

# Set up separate L1 caches for each CPU
for i in range(cpu_num):
    system.cpu[i].icache = L1ICache(size=options.l1i_size)
    system.cpu[i].dcache = L1DCache(size=options.l1d_size)
    system.cpu[i].l2bus = L2XBar()
    
    # Connect I & D caches to the CPU
    system.cpu[i].icache.connectCPU(system.cpu[i])
    system.cpu[i].dcache.connectCPU(system.cpu[i])

    # Connect caches to L2 bus
    system.cpu[i].icache.connectBus(system.cpu[i].l2bus)
    system.cpu[i].dcache.connectBus(system.cpu[i].l2bus)

    # Create and connect L2 caches to the memory bus
    system.cpu[i].l2cache = L2Cache(size=options.l2_size)
    system.cpu[i].l2cache.connectCPUSideBus(system.cpu[i].l2bus)
    system.cpu[i].l2cache.connectMemSideBus(system.membus)

# Instantiate and simulate the system
root = Root(full_system=False, system=system)
m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()

print('Exiting @ tick {} because {}'.format(m5.curTick(), exit_event.getCause()))
