#include <stdio.h>
#include <stdbool.h>
#include <instruccion.h>
#include <cspecs/cspec.h>


context (example) {

    describe("instruccion.h") {

        it("NO_OP 5 tiene parametro 5") {
            t_instruccion inst = obtenerInstruccion("NO_OP 5");
            should_int(inst.parametro1) be equal to(5);
            
        } end
         
        it("WRITE 4 42 tiene parametros 4 y 42") {
            t_instruccion inst = obtenerInstruccion("WRITE 4 42");
            should_int(inst.parametro2) be equal to(42);
            should_int(inst.parametro1) be equal to(4);
            
        } end

    } end

}
