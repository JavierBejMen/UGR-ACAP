#### Configuration ################################################################################
SRC = src
INC = inc
BIN = bin
DAT = data
OBJ = obj
DOC = doc
OUT = out

CC ?= gcc
CFLAGS := ${CFLAGS}
CFLAGS += -Wall -Wextra -pedantic
CFLAGS += -I $(INC)
LDFLAGS := ${LDFLAGS}
LDFLAGS += -lm

ifdef DEBUG
	CFLAGS += -g -O0
	LDFLAGS += -g
else
	CFLAGS += -O2
endif

MPI_CFLAGS = $(shell mpicc --showme:compile)
MPI_LDFLAGS += $(shell mpicc --showme:link)

TARGETS = clean $(BIN)/gauss_secuencial $(BIN)/mpi_gauss

.PHONY: clean run
.SILENT: clean run

all: $(TARGETS)

$(BIN)/gauss_secuencial : $(OBJ)/libbmp.o $(OBJ)/gauss.o $(OBJ)/gauss_secuencial.o
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ)/%.o : $(SRC)/%.c
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN)/mpi_gauss : $(OBJ)/libbmp.o $(OBJ)/mpi_gauss.o $(OBJ)/matrix_helper.o
	$(CC) $^ $(LDFLAGS) $(MPI_LDFLAGS) -o $@

$(OBJ)/%.o : $(SRC)/mpi_%.c
	$(CC) -c $< $(CFLAGS) $(MPI_CFLAGS) -o $@

run:
	#sh run_all.sh

clean:
	rm -rf $(OBJ)/*.o $(BIN)/* $(OUT)/* || true
