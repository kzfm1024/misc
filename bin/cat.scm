#!/usr/bin/env gosh

(define (main args)   ;entry point
  (if (null? (cdr args))
      (copy-port (current-input-port) (current-output-port))
      (for-each (lambda (file)
                  (call-with-input-file file
                    (lambda (in)
                      (copy-port in (current-output-port)))))
                (cdr args)))
  0)

#|
(define (cat)
  (port-for-each (lambda (byte) (write-byte byte)) read-byte))

(define (main args)
  (let ((files (cdr args)))
    (if (null? files)
        (cat)
        (for-each (lambda (file) (with-input-from-file file cat)) files))))
|#
