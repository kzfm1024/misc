#!/usr/bin/env gosh

(define (translate-token tk)
  (case tk
    [(#\+) (print "tape[cur] ||= 0; tape[cur] += 1")]
    [(#\-) (print "tape[cur] ||= 0; tape[cur] -= 1")]
    [(#\>) (print "cur += 1")]
    [(#\<) (print "cur -= 1")]
    [(#\[) (print "while tape[cur] != 0")]
    [(#\]) (print "end")]
    [(#\.) (print "print (tape[cur] || 0).chr")]
    [(#\,) (print "tape[cur] = $stdin.getc")]
    [else #f]))

(define (translate-line line)
  (for-each translate-token (string->list line)))

(define (translate)
  (port-for-each translate-line read-line))

(define (main args)
  (let ((files (cdr args)))
    (print "tape = []; cur = 0")
    (if (null? files)
        (translate)
        (for-each (lambda (file) (with-input-from-file file translate))
                  files))
    (print "puts \"\"")))
