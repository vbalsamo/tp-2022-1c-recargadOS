#include <stdio.h>
#include <stdbool.h>
#include <serializacion/estructuras.h>
#include <utils/planificador.h>
#include <cspecs/cspec.h>


context (example) {

    describe("t_pcb") {

        it("pcb es igual a pcb deserializado") {
            t_proceso * proceso = malloc(sizeof(t_proceso));
            proceso->tamanioProceso=1;
            proceso->sizeInstrucciones=2;
            proceso->instrucciones = malloc(sizeof(t_instruccion)*2);
            proceso->instrucciones[0].identificador = NO_OP;
            proceso->instrucciones[1].identificador = IO;
            
            
            t_pcb* pcb = iniciarPcb(proceso);
            void * stream = malloc(sizeof(uint32_t)*6 + proceso->sizeInstrucciones*(sizeof(uint32_t)*2 + sizeof(instruccion_id)));
            serializarPCB(stream, pcb, 0);
            t_pcb * deserializado = deserializarPCB(stream,0);
            should_int(pcb->id) be equal to(deserializado->id);
            should_int(pcb->tamanioProceso) be equal to(deserializado->tamanioProceso);
            should_int(pcb->PC) be equal to(deserializado->PC);
            should_int(pcb->tablaDePaginas) be equal to(deserializado->tablaDePaginas);
            should_int(pcb->estimacionRafaga) be equal to(deserializado->estimacionRafaga);
            should_int(pcb->lengthUltimaRafaga) be equal to(deserializado->lengthUltimaRafaga);
            should_int(pcb->sizeInstrucciones) be equal to(deserializado->sizeInstrucciones);
            should_int(pcb->instrucciones[0].identificador) be equal to(deserializado->instrucciones[0].identificador);
            should_int(pcb->instrucciones[0].parametro1) be equal to(deserializado->instrucciones[0].parametro1);
            should_int(pcb->instrucciones[0].parametro2) be equal to(deserializado->instrucciones[0].parametro2);
            should_int(pcb->instrucciones[1].identificador) be equal to(deserializado->instrucciones[1].identificador);
            should_int(pcb->instrucciones[1].parametro1) be equal to(deserializado->instrucciones[1].parametro1);
            should_int(pcb->instrucciones[1].parametro2) be equal to(deserializado->instrucciones[1].parametro2);
            
        } end

    } end

    describe("t_IO") {

        it("io es igual a io deserializado") {
            t_proceso * proceso = malloc(sizeof(t_proceso));
            proceso->tamanioProceso=1;
            proceso->sizeInstrucciones=2;
            proceso->instrucciones = malloc(sizeof(t_instruccion)*2);
            proceso->instrucciones[0].identificador = NO_OP;
            proceso->instrucciones[1].identificador = IO;
            
            t_IO * io = malloc(sizeof(t_IO));
            io->pcb = iniciarPcb(proceso);
            void * stream = malloc(sizeof(uint32_t)*7 + proceso->sizeInstrucciones*(sizeof(uint32_t)*2 + sizeof(instruccion_id)));
            serializarIO(stream, io);
            t_IO * deserializado = deserializarIO(stream);
            should_int(io->tiempoBloqueo) be equal to(deserializado->tiempoBloqueo);
            should_int(io->pcb->id) be equal to(deserializado->pcb->id);
            should_int(io->pcb->tamanioProceso) be equal to(deserializado->pcb->tamanioProceso);
            should_int(io->pcb->PC) be equal to(deserializado->pcb->PC);
            should_int(io->pcb->tablaDePaginas) be equal to(deserializado->pcb->tablaDePaginas);
            should_int(io->pcb->estimacionRafaga) be equal to(deserializado->pcb->estimacionRafaga);
            should_int(io->pcb->lengthUltimaRafaga) be equal to(deserializado->pcb->lengthUltimaRafaga);
            should_int(io->pcb->sizeInstrucciones) be equal to(deserializado->pcb->sizeInstrucciones);
            should_int(io->pcb->instrucciones[0].identificador) be equal to(deserializado->pcb->instrucciones[0].identificador);
            should_int(io->pcb->instrucciones[0].parametro1) be equal to(deserializado->pcb->instrucciones[0].parametro1);
            should_int(io->pcb->instrucciones[0].parametro2) be equal to(deserializado->pcb->instrucciones[0].parametro2);
            should_int(io->pcb->instrucciones[1].identificador) be equal to(deserializado->pcb->instrucciones[1].identificador);
            should_int(io->pcb->instrucciones[1].parametro1) be equal to(deserializado->pcb->instrucciones[1].parametro1);
            should_int(io->pcb->instrucciones[1].parametro2) be equal to(deserializado->pcb->instrucciones[1].parametro2);
  
        } end

    } end
}
