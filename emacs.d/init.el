;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  -*- coding: utf-8-unix -*-
;;  FSF Emacs 23 用 ユーザ設定ファイル
;;    Munehiro Yamamoto <munepi@cg8.so-net.ne.jp>
;;            $Id: .emacs.el,v 1.53 2009/04/20 18:32:24 daisuke Exp $
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; ;;; ユーザーの初期設定ファイルの指定
;; (setq user-init-file "~/.emacs.el")
;; (setq custom-file "~/.emacs-custom.el")

;; ;;; マクロサーチパスの追加
;; ;;; ~/lib/emacs 以下にユーザ用の *.el, *.elc を置くことができます
;; (setq load-path (append '("~/lib/emacs") load-path))

;;; 以下、オプションにて手作業で加えた部分
;;; スクロールバーを右側に表示する
(set-scroll-bar-mode 'right)

;;; 時刻を表示する
(display-time)

;;; 行番号を表示する
(line-number-mode t)

;;; 起動時の画面はいらない
(setq inhibit-startup-message t)

;;; anthyをロード(anthy-elをインストールする必要あり)
;(load-library "anthy")
;(setq default-input-method "japanese-anthy")
;(if (>= emacs-major-version 23)
;  (setq anthy-accept-timeout 1))

;;; 全角/半角キーで日本語入力できるようにする
;(global-set-key [zenkaku-hankaku] 'toggle-input-method)

;;; ファイルオープン時に、ファイルダイアログを表示させずに、
;;; ミニバッファにて入力できるようにする
(setq use-file-dialog nil)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; X 版 Emacs のフォント/カラー設定
;; .emacs-faces.el, .emacs-fontset.el 参照
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;(if (>= emacs-major-version 23)
;    (cond (window-system
;           (setq initial-frame-alist
;		 (append (list
;			  '(width . 40)
;			  '(height . 35)
;			  '(top . 0)
;			  '(left . 0)
;			  )
;			 initial-frame-alist)
;		 )
;	   (setq default-frame-alist initial-frame-alist)
;	   )))

(load "~/.emacs.d/faces.el")

;;; オプションにて手作業で加えた部分ここまで

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; （旧版）ユーザ用初期化ファイル
;; ~/.emacs.my.el に個人用設定を書けます。
;; このファイルを直接いじりたくない場合は、個人設定を分離してください。
(if (file-exists-p (expand-file-name "~/.emacs.d/my.el"))
    (load (expand-file-name "~/.emacs.d/my.el") nil t nil))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Local Variables:
;; mode: emacs-lisp
;; End:
