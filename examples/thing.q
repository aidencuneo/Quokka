import * 'builtins'
println('-' * (max(map((lam 1 : '| ' + argv[0] + ' |'), (lam 1 : argv[0].upper()), println, cpad(['words', 'more words', 'yup', 'this is a sentence', 'these words will be aligned']))) - 1))
