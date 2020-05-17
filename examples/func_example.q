fun doabsolutelynothing
end

fun donothing 0
end

fun printonething 1
    println(args[0])
end

fun printanything 1 to *
    for i : args
        println(i)
    end
end

fun printsomethings 1 to 4
    for i : args
        println(args[i])
    end
end
