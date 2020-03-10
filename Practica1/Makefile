#### Configuration ################################################################################
SRC = src
INC = include
BIN = bin
DAT = data
OBJ = obj

CC ?= gcc
CFLAGS := ${CFLAGS}
CFLAGS += -Wall -Wextra -pedantic -O3
CFLAGS += -I $(INC)
LDFLAGS := ${LDFLAGS}

MPI_CFLAGS = $(shell mpicc --showme:compile)
MPI_LDFLAGS += $(shell mpicc --showme:link)

TARGETS = clean $(BIN)/pi_secuencial_estudio $(BIN)/pi_concurrente $(BIN)/pi_secuencial
##### Dependency Rules ############################################################################

.PHONY: clean run
.SILENT: clean run

all: $(TARGETS)

$(BIN)/pi_secuencial : $(OBJ)/pi_secuencial.o
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ)/pi_secuencial.o : $(SRC)/pi_secuencial.c
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN)/pi_secuencial_estudio : $(OBJ)/pi_secuencial_estudio.o
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ)/pi_secuencial_estudio.o : $(SRC)/pi_secuencial_estudio.c
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN)/pi_concurrente :: $(OBJ)/pi_concurrente.o
	$(CC) $^ $(LDFLAGS) $(MPI_LDFLAGS) -o $@

$(OBJ)/%.o : $(SRC)/%.c
	$(CC) -c $< $(CFLAGS) $(MPI_CFLAGS) -o $@

run:
	sh run_all.sh

clean:
	rm -rf $(OBJ)/*.o $(BIN)/* $(DAT)/*