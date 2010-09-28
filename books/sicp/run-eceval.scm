(load "./ch5-regsim.scm")
(load "./ch5-eceval-support.scm")
(load "./ch5-eceval.scm")

(define the-global-environment (setup-environment))
(start eceval)

;; To restart, can do just
;: (start eceval)
