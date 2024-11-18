import m5
from m5.objects import *

# Create a system
system = System()

# Set the clock for the system
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '2GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Set the memory mode
system.mem_mode = 'timing'  # Use 'timing' for a more detailed simulation

# Configure memory
system.mem_ranges = [AddrRange('512MB')]

# Create multiple CPU cores
num_cores = 4
system.cpu = [MinorCPU() for i in range(num_cores)] # TimingSimpleCPU()

# Create a memory controller
system.membus = SystemXBar()

# Connect the CPU ports to the memory bus
for cpu in system.cpu:
    cpu.icache_port = system.membus.cpu_side_ports
    cpu.dcache_port = system.membus.cpu_side_ports

# Create a memory controller for DRAM
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports
system.system_port = system.membus.cpu_side_ports

# Setup the workload
path = os.path.dirname(os.path.realpath(__file__))
binary = os.path.join(
    path,
    "../../../",
    "tests/test-progs/daxpy_operations/bin/daxpy",
)  # Path to your compiled DAXPY program
system.workload = SEWorkload.init_compatible(binary)

# Create processes for each core
for i in range(num_cores):
    process = Process()
    process.cmd = [binary]
    system.cpu[i].workload = process
    system.cpu[i].createThreads()

# Instantiate the system
root = Root(full_system=False, system=system)
m5.instantiate()

print("Starting simulation...")
exit_event = m5.simulate()
print(f"Simulation ended at tick {m5.curTick()} because {exit_event.getCause()}")
