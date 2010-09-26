#!/usr/bin/env gosh

;(use gauche.parseopt)

(define *delim* #\x09)

(define (cut k)
  (port-for-each
   (lambda (line)
     (print (list-ref (string-split line *delim*) (- k 1))))
   read-line))

(define (main args)
  (let ((k (string->number (cadr args)))
        (files (cddr args)))
    (if (null? files)
        (cut k)
        (for-each (lambda (file)
                    (with-input-from-file file (lambda () (cut k))))
                  files))))
