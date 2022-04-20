#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>


context (default) {

    describe("default") {

        it("repalce") {
            should_int(5) be equal to(5);
        } end
       

    } end

}
