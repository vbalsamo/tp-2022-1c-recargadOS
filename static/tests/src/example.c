#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>
#include <instruccion.h>

context (example) {

    describe("Hello world") {

        it("true should be true") {
            t_instruccion* inst = obtenerInstruccion("NO_OP 5");
            should_bool(inst->parametro1) be equal to(5);
        } end
         
        it("true shouldn't be false") {
            should_bool(true) not be equal to(false);
        } end

        skip("this test will fail because \"Hello\" is not \"Bye\"") {
            should_string("Hello") be equal to("Bye");
        } end

    } end

}
