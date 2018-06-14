(define (node-new type key str left right parent)
  (list type key str left right parent))

(define (nth-elt node n)
  (if (<= n 1) (car node) (nth-elt (cdr node) (- n 1))))

(define node-type
  (lambda (node) (nth-elt node 1)))

(define node-key
  (lambda (node) (nth-elt node 2)))

(define node-str
  (lambda (node) (nth-elt node 3)))

(define node-left
  (lambda (node) (nth-elt node 4)))

(define node-right
  (lambda (node) (nth-elt node 5)))

(define node-parent
  (lambda (node) (nth-elt node 6)))

; TODO:
; 
