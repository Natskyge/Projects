; 
; Elementary functions
; (cond (p0 e0) (p1 e1) ... (pn en)) -> The first ei for 0<=i<=n and pi=#t
;                                       if there is no such pi then it does
;                                       nothing.
;
; (eq? X X) -> #t
; (eq? X Y) -> #f
; (eq? X '(X Y)) -> error 
;
; (car X) -> error 
; (car '(X Y)) -> X
; (car '((X Y) Z)) -> (X Y)
;
; (cdr '(X Y)) -> X
; (cdr '((X Y) Z)) -> Z
;
; (cons X Y) -> (X Y)
; (cons '((X Y) Z)) -> ((X Y) Z) 
;
;
; Some basic relations
; (car (cons X Y)) -> X
; (cdr (cons X Y)) -> Y
; (cons (car 'X) (cdr 'X)) -> 'X

; From these we may define the following

; and
(define (and. p q)
  (cond (p q)
        (#t #f)))

; or
(define (or. p q)
  (cond (p #t)
        (#t q)))

; not
(define (not. p)
  (cond (p #f)
        (#t #t)))

; atom? which gives #t if x is an atom and #f if not
(define (atom? x)
  (and. (not. (pair? x))
        (not. (null? x))))

; Let x be an S-exp, the (ff x) gives the first atomic symbol of x 
(define (ff x)
  (cond ((atom? x) x)
        (#t (ff (car x)))))

; Let x and y be S-exp and z be atomic, them (subst x y z) returns the result of
; substituting all occurrences of z with x in y.
(define (subst x y z)
  (cond (atom? z (cond ((eq? z y) x)
                       (#t z)))
        (#t (cons (subst x y (car z))
                  (subst x y (cdr z))))))

; Let x and y be S-exp, then (equal x y) -> #t if they are the same and #f
; otherwise 
(define (equal x y)
  (or (and (atom? x) 
           (atom? y) 
           (eq? x y))
      (and (not (atom? x))
           (not (atom? y))
           (equal (car x) (car y))
           (equal (cdr x) (cdr y)))))

; Common compositions of car and cdr
(define (cadr. x)
  (car (cdr x)))

(define (caddr. x)
  (car (cdr (cdr x))))

; Indeed we may define the n'th of such a function
(define (n-cadr x n)
  (cond ((eq? n 0)
         (car x))
        (n-cadr (cdr x) (- n 1))))

; Let x and y be S-exp, then (append. x y) returns the result of appending y to
; x
(define (append. x y)
  (cond ((null? x) y)
        (#t (cons (car x)
                  (append. (cdr x) y)))))

; Let x and y be S-exp, then (among x y) -> #t if x is occurs in y. 
(define (among x y)
  (and. (not. (null? y))
        (or. (equal x (car y))
             (among x (cdr y)))))

; Let x and y be S-exp, then (pair x y) gives corresponding elements of x and y 
(define (pair x y)
  (cond ((and. (null? x)
               (null? y))
         '())
        ((not. (or. (atom? x)
                    (atom? y)))
         (cons (list (car x) (car y))
               (pair (cdr x) (cdr y))))))

; Let y='((u1 v1) ... (un vn)) then (assoc. ui y) returns the corresponding vi
; for ui and 1<=i<=n 
(define (assoc. ui y)
  (cond ((eq? (caar y)
              ui)
         (cadar y))
        (#t (assoc. ui (cdr y)))))

; Let x='((u1 vi) ... (un vn)) where all ui for 1<=i<=n are atomic and y be any
; S-exp, then (sublis x y) gives the result of substituting all ui in y with vi.
; First we define an auxiliary function sub2:
(define (sub2 x z)
  (cond ((null? x) z)
        ((eq? (caar x) z)
         (cadar x))
        (#t (sub2 (cdr x) z))))

; and
(define (sublis x y)
  (cond ((atom? y) (sub2 x y))
        (#t (cons (sublis x (car y))
                  (sublis x (cdr y))))))
