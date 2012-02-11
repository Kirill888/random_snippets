; Solution to a puzzle I got from Derek (aparently from google interview) 
; I no longer remember exactly the problem definition
; ...should have written it down...
;
; It is something along these lines:
;  Given a positive integer X find all non-increasing sequences of
;  positive integers that would add up to X.
;  And you have to print them out in a particular order also
;  (shortest to longest, but also {4 1} before {3 2}, not sure
;   how exactly this was worded in the original)
; Example: 3 -> {3} {2 1} {1 1 1}
;
;
; This is Emacs lisp, do this in your emacs to see it in action:
;   M-x eval-buffer
; To try different inputs do:
;   M-x puzzle-ask
;


(defun puzzle (p1 &optional p2 lvl prefix)
  (if (not p2)
      (if lvl (progn (puzzle (- p1 lvl) lvl lvl "")
		     (if (< lvl p1) (puzzle p1 () (+ 1 lvl))))
	(puzzle p1 () 0))
    (if (> lvl 0)
	(let ( (ss (format "%s%d " prefix p1)) (l (- lvl 1)) n1 n2 )
	  (setq n1 (min p1 (- p2 l)) )
	  (setq n2 (- p2 n1))
	  (if (>= (* n1 l  ) n2 ) (puzzle n1 n2 l ss ))
	  (setq p2 (+ p2 1))
	  (setq p1 (- p1 1))
	  (if (>= (* p1 lvl) p2 ) (puzzle p1 p2 lvl prefix )))
      (message "%s%d" prefix p1))))

(defun puzzle-ask (v) 
  (interactive "NEnter Number:" )
  (puzzle v)
  (switch-to-buffer "*Messages*")
  (end-of-buffer))

(puzzle-ask 5)

;Expect to see folowing in your message buffer:
;5
;4 1
;3 2
;3 1 1
;2 2 1
;2 1 1 1
;1 1 1 1 1
