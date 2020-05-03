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
    string data = fileread('file.txt')
    println data
end
