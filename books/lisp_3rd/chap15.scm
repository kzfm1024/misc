(define power-of-two
  (let ((previous-power-of-two 1))
    (lambda ()
      (set! previous-power-of-two
            (* previous-power-of-two 2))
      previous-power-of-two)))
