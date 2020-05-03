def string strcat(string word, integer count)
    string out
    rangefor _ count
        out = out + word
    end
    ret out
end

main:
    println strcat('aiden', 3)
end
