;; デフォルトのフォントセットなど
(if (or (and window-system (>= emacs-major-version 19))
	(>= emacs-major-version 21))
    (progn

      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; フォント/ウィンドウサイズの設定

;      (if window-system
;	  (progn
	    ;(load "~/.emacs.d/fontset.el")
;	    (setq default-frame-alist
;		  (append
		   ;'((font . "fontset-14") ;; デフォルトフォントセット
		     ;(width . 80) (height . 40) ;; ウィンドウサイズ
		    ; )
;		   default-frame-alist))))

      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      ;; font-lock-mode
      ;; 色をつける
      (global-font-lock-mode t)
      ;; font-lockでの装飾レベル
      (setq font-lock-maximum-decoration t) 
      (setq fast-lock nil)
      (setq lazy-lock nil)
      (setq jit-lock t)

      ;; hilit19 の設定
      ;; 背景色が明るい色
      ;;(setq hilit-background-mode 'light)
      ;; 背景色が暗い色
      (setq hilit-background-mode 'dark)
      ;; hilit19 の読み込み
      ;(require 'hilit19)
      
      ;(set-background-color "grey80")         ;; 明るい背景色(Emacs20)
      ;(set-face-background 'default "grey80") ;; 明るい背景色
      ;(set-face-foreground 'default "black")  ;; 暗い文字色
      (set-background-color "#333366")         ;; 標準の背景色(Emacs20)
      (set-face-background 'default "#333366") ;; 標準の背景色
      (set-face-foreground 'default "white")   ;; 標準の文字色
      (if (and window-system (>= emacs-major-version 21))
	  (progn
	    (set-face-background 'menu "grey94")        ;; メニュー
	    (set-face-background 'tool-bar "grey94")    ;; ツールバー
	    (set-face-background 'mode-line "grey94")   ;; モードライン
	    (set-face-background 'scroll-bar "grey94")  ;; スクロールバー
	    (set-face-foreground 'tooltip "black")  ;; ツールチップ
	    (set-face-background 'tooltip "ghostwhite")  ;; ツールチップ
            ))
      (set-cursor-color "white")

      (if (not window-system)
          (progn
            ;; 非X環境での色設定
            (set-face-background 'default "000000")
            (set-face-foreground 'default "ffffff")))
      )
)
