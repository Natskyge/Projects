(define (make-node data leftNode rightNode)
  (list data leftNode rightNode))

(define root (make-node 0 0 0))

(define tree root)

(define left-conection cadr)

(define right-conection caddr)

(define data-node car)

(define (create-node rootnode side data)
  (cond ((= side 1)
         (make-node (data-node rootnode)
                    (right-conection rootnode)
                    (make-node data 0 0)))
        ((= side 0)
          (make-node (data-node rootnode)
                     (make-node data 0 0)
                     (left-conection rootnode)))))

(define (find-node level row tree)
  (define maxLeft 0)
  (begin (set! maxLeft (- (expt 2 (- level 1)) 1))
         (cond ((= 0 level row) tree)
               ((< maxLeft row)
                (find-node (- level 1)
                           (- (- row 1) maxLeft)
                           (right-conection tree)))
               (#t
                (find-node (- level 1)
                           row
                           (left-conection tree))))))

(define (insert-leaf level row tree data)
  (define maxLeft 0)
  (begin (set! maxLeft (- (expt 2 (- level 1)) 1))
         (cond ((= 0 level row) (make-node data 0 0))
               ((< maxLeft row)
                (make-node (data-node tree)
                           (left-conection tree)
                           (insert-leaf (- level 1)
                                        (- (- row 1) maxLeft)
                                        (right-conection tree)
                                        data)))
               (#t
                (make-node (data-node tree)
                           (insert-leaf (- level 1)
                                        row
                                        (left-conection tree)
                                        data)
                           (right-conection tree))))))

(define (insert-node level row data)
  (set! tree (insert-leaf level row tree data)))
