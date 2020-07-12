fun strsum 1
    c = ''
    i = 0
    while i < len(argv[0])
        c += argv[0][i++]
    end
    ret c
end

fun html *
    ret '<html>\n' + strsum(argv) + '</html>\n'
end

fun head *
    ret '<head>\n' + strsum(argv) + '</head>\n'
end

fun title 1
    ret '<title>' + argv[0] + '</title>\n'
end

fun body *
    ret '<body>\n' + strsum(argv) + '</body>\n'
end

fun p *
    ret '<p>' + strsum(argv) + '</p>\n'
end

fun b *
    ret '<b>' + strsum(argv) + '</b>\n'
end

fun div *
    ret '<div>' + strsum(argv) + '</div>\n'
end

SOURCE = html(
    title('This is a website'),
    body(
        p('Hey'),
    ),
)

println(SOURCE)
