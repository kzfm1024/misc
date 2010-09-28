#!/usr/bin/env gosh

(load "./ch4-query.scm")
(initialize-data-base microshaft-data-base)
(query-driver-loop)

;;
;; 各手続きの動作を調べてみる
;;

;
; query-syntax-process は ?x を (? x) に変換
;
; gosh> (query-syntax-process '(job ?x (computer programmer)))
; (job (? x) (computer programmer))
;

;
; gosh> (display-stream (simple-query '(job (? x) (computer programmer))
;                                     (singleton-stream '())))
;
; (((? x) Fect Cy D))
; (((? x) Hacker Alyssa P))done
;

#|
(display-stream (simple-query '(lives-near (? x) (Bitdiddle Ben))
                              (singleton-stream '())))

=> 生成されたフレーム
(((? 2 rest-2) (Ridge Road) 10)
 ((? 2 rest-1) (Onion Square) 5)
 ((? 2 town) . Slumerville)
 ((? 2 person-1) Aull DeWitt)
 ((? 2 person-2) Bitdiddle Ben)
 ((? x) ? 2 person-1))
(((? 2 rest-2) (Ridge Road) 10)
 ((? 2 rest-1) (Pine Tree Road) 80)
 ((? 2 town) . Slumerville)
 ((? 2 person-1) Reasoner Louis)
 ((? 2 person-2) Bitdiddle Ben)
 ((? x) ? 2 person-1))
|#

;
; fetch-assertions は THE-ASSERTIONS から表明を取得
; patter の先頭が定数の場合は get-indexed-assertions でマッチする要素のストリームを返す
; 
; gosh> (display-stream (get-indexed-assertions '(job (? x) (computer programmer))))
; 
; (job (Aull DeWitt) (administration secretary))
; (job (Cratchet Robert) (accounting scrivener))
; (job (Scrooge Eben) (accounting chief accountant))
; (job (Warbucks Oliver) (administration big wheel))
; (job (Reasoner Louis) (computer programmer trainee))
; (job (Tweakit Lem E) (computer technician))
; (job (Fect Cy D) (computer programmer))
; (job (Hacker Alyssa P) (computer programmer))
; (job (Bitdiddle Ben) (computer wizard))done

;
; (pattern-match '(job (? x) (computer programmer))
;                '(job (Hacker Alyssa P) (computer programmer))
;                '())
; => (((? x) Hacker Alyssa P))
;

;
; gosh> (display-stream (find-assertions '(job (? x) (computer programmer)) '()))
;
; (((? x) Fect Cy D))
; (((? x) Hacker Alyssa P))done
