(defmacro when-plusp-with-backquote (number result)
  `(when (plusp ,number) ,result))

(defmacro when-plusp-with-rest (number &rest rest)
  `(when (plusp ,number) ,@rest))
