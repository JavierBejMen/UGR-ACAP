INC = inc

NVCC = nvcc
NVCFLAGS := ${CFLAGS}
NVCFLAGS += -m64

TARGETS = info suma_cpu suma_gpu

all: clean $(TARGETS)

.PHONY: clean

%: %.cu
	$(NVCC) $(NVCFLAGS) -o $@ $< -I$(INC)

%: %.c
	gcc -O2 -lm -o $@ $<


clean:
	rm $(TARGETS) || true
	rm *.png || true
	rm data_*.dat || true
