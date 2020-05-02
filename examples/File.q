object
    `FILE * fp`
    string path
    string mode
    bool exists
    bool opened

    def void write(string text)
    def string read
    def void close
end

class(string path, string mode)

    self.path = path
    self.mode = mode
    self.fp = `fopen(path.value, mode.value)`

    self.exists = `Bool(!!self.fp)`

    self.opened = true

    def void write(string text)
        bool doesntexist = false
        `if (!self.exists.value.value) doesntexist = Bool(1)`

        if doesntexist
            raise("file '" + self.path + "' not found")
        end

        bool notvalidwrite = false
        `if (!self.opened.value.value) notvalidwrite = Bool(1)`

        if notvalidwrite
            raise('attempted write operation on a closed file')
        end

        `fprintf(self.fp, "%s", text.value)`
    end

    def string read
        bool doesntexist = false
        `if (!self.exists.value.value) doesntexist = Bool(1)`

        if doesntexist
            raise("file '" + self.path + "' not found")
        end

        bool notvalidread = false
        `if (!self.opened.value.value) notvalidread = Bool(1)`
        
        if notvalidread
            raise('attempted read operation on a closed file')
        end

        `char * buffer = 0`
        `long length`

        `fseek(self.fp, 0, SEEK_END)`
        `length = ftell(self.fp)`
        `fseek(self.fp, 0, SEEK_SET)`
        `buffer = malloc(length)`
        `if (buffer) fread(buffer, 1, length, self.fp)`

        ret `String(buffer)`
    end

    def void close
        `fclose(self.fp)`
        self.opened = false
    end

end
