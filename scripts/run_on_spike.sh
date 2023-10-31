# Run tests on spike emulator
# 
# Assumes spike is in PATH

# Build tests
make clean;
export PLAT=spike LOG_LEVEL=LOG_WARNING;
make

# Run tests
mkdir -p log
spike -l --log-commits --log=log/spike.log --isa=RV64gch build/spike/rvh_test.elf