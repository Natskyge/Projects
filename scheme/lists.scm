(define (last-pair lst)
  (cond ((null? (cdr lst)) lst)
        (#t (last-pair (cdr lst)))))

(define (reverse lst)
  (cond ((null? lst) '())
        (#t (cons (reverse (cdr lst)) (car lst)))))
