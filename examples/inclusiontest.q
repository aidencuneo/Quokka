include:
    File
end

def string fileread(string path)
    File fileobj = new File(path, 'r')
    string data = fileobj.read()
    fileobj.close()

    ret data
end

main:
    File textfile = new File('file.txt', 'r')
    println textfile.read()
    textfile.close()
end
