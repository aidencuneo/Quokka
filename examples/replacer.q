object reverse_int

    integer val

    def bool __bool__

    def reverse_int __add__(integer other)
    def reverse_int __sub__(integer other)

end

class reverse_int(integer value)

    self.val = value

    def bool __bool__
        if self.val
            ret true
        end
        ret false
    end

    def reverse_int __add__(integer other)
        ret new reverse_int(self.val - other)
    end

    def reverse_int __sub__(integer other)
        ret new reverse_int(self.val + other)
    end

end

main:
    reverse_int num = new reverse_int(20000)
    while num
        println num.val
        num = num + 1
    end
end
