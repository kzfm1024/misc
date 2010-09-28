;;
;; Excercise 4.1
;;

;; from left to right
(define (list-of-values exps env)
  (if (no-operands? exps)
      '()
      (let* ((left (eval (first-operand exps) env))
             ((right (list-of-values (rest-operands exps) env))))
        (cons left right))))

;; from right to left
(define (list-of-values exps env)
  (if (no-operands? exps)
      '()
      (let* ((right (list-of-values (rest-operands exps) env))
             ((left (eval (first-operand exps) env))))
        (cons left right))))
