#!/usr/bin/evn gosh

(add-load-path "/Users/kzfm1024/svn/books/esoteric/Whitespace/lib")
(use whitespace.compiler)
(use whitespace.vm)

(define (run-whitespace)
  (run (compile (port->string (current-input-port)))))

(define (main args)
  (let ((files (cdr args)))
    (if (null? files)
        (run-whitespace)
        (for-each (lambda (file) (with-input-from-file file run-whitespace))
                  files))))
