// Settings
str = 'string'
recursion_count = 6

strbeg = ''
strmid = disp(str)
strend = ''

a = 0
until a == recursion_count
    strbeg += 'disp('
    strend += ')'

    a += 1
end

code = strbeg + strmid + strend

exec('c = ' + code)
println(c)
