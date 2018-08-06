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

;; Sicp style
(define (entry tree) (car tree))

(define (left-branch tree) (cadr tree))

(define (right-branch tree) (caddr tree))

(define (make-tree entry left right)
  (list entry left right))

(define (element-of-set? x set)
  (cond ((null? set) #f)
		((= x (entry set)) #t)
		((< x (entry set))
		 (element-of-set? x (left-branch set)))
		((> x (entry set))
		 (element-of-set? x (right-branch set)))))

(define (adjoin-set x set)
  (cond ((null? set) (make-tree x '() '()))
		((= x (entry set)) set)
		((< x (entry set))
		 (make-tree (entry set)
					(adjoin-set x (left-branch set))
					(right-branch set)))
		((> x (entry set))
		 (make-tree (entry set)
					(left-branch set)
					(adjoin-set x (right-branch set))))))

(define tree
  (make-tree 4 
	(make-tree 2 
			   (make-tree 1 '() '())
			   (make-tree 3 '() '()))
	(make-tree 6
			   (make-tree 5 '() '())
			   (make-tree 7 '() '()))))

; O(n log n)
(define (tree->list-1 tree)
  (if (null? tree)
	  '()
	  (append (tree->list-1 (left-branch tree))
			  (cons (entry tree)
					(tree->list-1 (right-branch tree))))))

; O(n)
(define (tree->list-2 tree)
  (define (copy-to-list tree result-list)
	(if (null? tree)
	    result-list
		(copy-to-list (left-branch tree)
					  (cons (entry tree)
							(copy-to-list (right-branch tree)
										  result-list)))))
  (copy-to-list tree '()))

(define (list->tree elements)
  (car (partial-tree elements (length elements))))

(define (partial-tree elts n)
  (if (= n 0)
	  (cons '() elts)
	  (let ((left-size (quotient (- n 1) 2)))
		(let ((left-result (partial-tree elts left-size)))
		  (let ((left-tree (car left-result))
				(non-left-elts (cdr left-result))
				(right-size (- n (+ left-size 1))))
			(let ((this-entry (car non-left-elts))
				  (right-result (partial-tree (cdr non-left-elts)
											  right-size)))
			  (let ((right-tree (car right-result))
					(remaning-elts (cdr right-result)))
				(cons (make-tree this-entry left-tree right-tree)
					  remaning-elts))))))))

; Exercise 2.64
; a) It recursivly makes a tree of the left half, then of the remaning right
; half and then combines and returns them useing make-tree where the entry is
; from the root note, adding with cons the remaning elements.
;                            5
;                           / \
;                          1   9
;                           \  |\
;                            3 7 11
; b) O(n)

(define (lookup given-key tree)
  (cond ((null? tree) #f)
		((= given-key (key (entry tree)))
		 (entry tree))
		((< given-key (key (entry tree)))
		 (lookup given-key (left-branch tree)))
		(else
		 (lookup given-key (right-branch tree)))))

(define (make-leaf symbol weight)
  (list 'leaf symbol weight))

(define (leaf? object)
  (eq? (car object) 'leaf))

(define (symbol-leaf x) (cadr x))

(define (weight-leaf x) (caddr x))

(define (make-code-tree left right)
  (list left
		right
		(append (symbols left) (symbols right))
		(+ (weight left) (weight right))))

(define (left-branch tree) (car tree))

(define (right-branch tree) (cadr tree))

(define (symbols tree)
  (if (leaf? tree)
	  (list (symbol-leaf tree))
	  (caddr tree)))

(define (weight tree)
  (if (leaf? tree)
	  (weight-leaf tree)
	  (cadddr tree)))

(define (choose-branch bit branch)
  (cond ((= bit 0) (left-branch branch))
		((= bit 1) (right-branch branch))
		(else (error "bad bit -- CHOOSE-BRANCH" bit))))

(define (decode bits tree)
  (define (decode-1 bits current-branch)
	(if (null? bits)
	    '()
		(let ((next-branch
			   (choose-branch (car bits) current-branch)))
		  (if (leaf? next-branch)
			  (cons (symbol-leaf next-branch)
					(decode-1 (cdr bits) tree))
			  (decode-1 (cdr bits) next-branch)))))
  (decode-1 bits tree))

(define (adjoin-set x set)
  (cond ((null? set) (list x))
		((< (weight x) (weight (car set))) (cons x set))
		(else (cons (car set)
					(adjoin-set x (cdr set))))))

(define (make-leaf-set pairs)
  (if (null? pairs)
	  '()
	  (let ((pair (car pairs)))
		(adjoin-set (make-leaf (car pair)   ; Symbol
							   (cadr pair)) ; Frequency
					(make-leaf-set (cdr pairs))))))

(define sample-tree
  (make-code-tree (make-leaf 'A 4)
				  (make-code-tree
					(make-leaf 'B 2)
					(make-code-tree (make-leaf 'D 1)
									(make-leaf 'C 1)))))

(define sample-message '(0 1 1 0 0 1 0 1 0 1 1 1 0))

; (decode sample-message sample-tree) => (A D A B B C A)

(define (check-in symbol lst)
  (cond ((null? lst) #f)
		((eq? symbol (car lst)) #t)
		(else
		  (check-in symbol (cdr lst)))))

(define (encode-symbol symbol tree)
  (cond ((not (pair? tree))
		 (error "symbol not in tree -- ENCODE-SYMBOL" symbol))
		((and (leaf? tree) (eq? symbol (symbol-leaf tree)))
		 '())
		((check-in symbol (symbols (left-branch tree)))
		 (cons 0 (encode-symbol symbol (left-branch tree))))
		(else
		 (cons 1 (encode-symbol symbol (right-branch tree))))))

(define (encode message tree)
  (if (null? message)
	  '()
	  (append (encode-symbol (car message) tree)
			  (encode (cdr message) tree))))

(define (generate-huffman-tree pairs)
  (successive-merge (make-leaf-set pairs)))

(define (successive-merge leaf-set)
  (if (null? (cdr leaf-set))
	  (car leaf-set)
	  (let ((e1 (car leaf-set))
			(e2 (cadr leaf-set)))
		(successive-merge (adjoin-set (make-code-tree e1 e2) (cddr leaf-set))))))

(define rock-tree 
  (generate-huffman-tree 
	'((A 2) (BOOM 1) (GET 2) (JOB 2) (NA 16) (SHA 3) (YIP 9) (WAH 1))))

(make-leaf-set '((A 2) (BOOM 1) (GET 2) (JOB 2) (NA 16) (SHA 3) (YIP 9) (WAH 1)))


(length (append '(1 1 1 1 1 1 1 0 0 1 1 1 1 0)
				'(1 1 1 0 0 0 0 0 0 0 0 0)
				'(1 1 1 1 1 1 1 0 0 1 1 1 1 0)
				'(1 1 1 0 0 0 0 0 0 0 0 0)
				'(1 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0)
				'(1 1 1 0 1 1 0 1 1)))

(decode '(1 1 1 1 1 1 1 0 0 1 1 1 1 0) rock-tree)
(decode '(1 1 1 0 0 0 0 0 0 0 0 0)  rock-tree)
(decode '(1 1 1 1 1 1 1 0 0 1 1 1 1 0) rock-tree)
(decode '(1 1 1 0 0 0 0 0 0 0 0 0) rock-tree)
(decode '(1 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0) rock-tree)
(decode '(1 1 1 0 1 1 0 1 1) rock-tree)

(define alpha-tree
  (generate-huffman-tree
	'((#\space 13.00)
	 (#\E 12.02)
	 (#\T 9.10)
	 (#\A 8.12)
	 (#\O 7.68)
	 (#\I 7.31)
	 (#\N 6.95)
	 (#\S 6.28)
	 (#\R 6.02)
	 (#\H 5.92)
	 (#\D 4.32)
	 (#\L 3.98)
	 (#\U 2.88)
	 (#\C 2.71)
	 (#\M 2.61)
	 (#\F 2.30)
	 (#\Y 2.11)
	 (#\W 2.09)
	 (#\G 2.03)
	 (#\P 1.82)
	 (#\B 1.49)
	 (#\V 1.11)
	 (#\K 0.69)
	 (#\X 0.17)
	 (#\Q 0.11)
	 (#\J 0.10)
	 (#\Z 0.07))))

(define (encode-string str)
  (encode (string->list str) alpha-tree))

(define (decode-string bits)
  (list->string (decode bits alpha-tree)))

(define bit-tree
  (generate-huffman-tree
	'((00 1)
	  (01 2)
	  (10 3)
	  (11 4))))


00 1
01 2
10 3
11 4
