(define (cont-frac n d k)
  (define (cont-frac-rec i)
    (if (< k i)
        0
        (/ (n i)
           (+ (d i) (cont-frac-rec (+ i 1))))))
  (cont-frac-rec 1))

(define (cont-frac-it n d k)
  (define (map x i)
    (if (< i 1)
        x
        (map (/ (n i) (+ (d i) x)) (- i 1))))
  (map 0 k))

(define (euler k)
  (+ 2
     (cont-frac-it (lambda (i) 1.0)
                   (lambda (i) (if (= 2 (modulo i 3))
                                   (/ (+ i 1) 1.5)
                                   1))
                   k)))

(define (tan x k)
  (cont-frac-it (lambda (i) (if (= i 1) x (- 0 (* x x))))
                (lambda (i) (- (* 2 i) 1))
                k))
