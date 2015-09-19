# Cmake script to run the Cgreen ${runner} on a ${lib}
# save the output as ${actual}, then strip it from ${remove}
# and then compare it to ${expected}

MESSAGE(runner = ${runner})

execute_process(
    COMMAND ${runner} ${lib}
    OUTPUT_FILE ${actual}
)

execute_process(
    COMMAND sed -e s%${remove}%%g ${actual}
    OUTPUT_FILE ${actual}.tmp
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E rename ${actual}.tmp ${actual}
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E compare_files ${expected} ${actual}
    RESULT_VARIABLE comparison_failed    
)

if(${comparison_failed})
    execute_process(COMMAND diff -c ${actual} ${expected} OUTPUT_VARIABLE diff)
    MESSAGE(SEND_ERROR "${diff}")
endif()