def string strgetch(string s, integer i)
    ret `String(s.value[i.value])`
end

def integer file_last_modified(string path)
    `if (getrealpath(path.value) == 0) return Integer(0)`

    `struct stat attr`
    `stat(path.value, &attr)`
    `struct tm * gmt = gmtime(&(attr.st_ctime))`

    `struct tm t = {0}`  // Initalize to all 0's
    `t.tm_year = gmt->tm_year - 1900`  // This is year-1900, so 112 = 2012
    `t.tm_mon = gmt->tm_mon - 1`
    `t.tm_mday = gmt->tm_mday`
    `t.tm_hour = gmt->tm_hour`
    `t.tm_min = gmt->tm_min`
    `t.tm_sec = gmt->tm_sec`
    `time_t timeSinceEpoch = mktime(&t)`
    ret `Integer(timeSinceEpoch)`
end

def string strexp(string st, integer exp)
    string out
    rangefor _ exp
        out += st
    end
    ret out
end
