(add-to-list 'load-path "~/.emacs.d/")
(autoload 'glsl-mode "glsl-mode" nil t)
(add-to-list 'auto-mode-alist '("\\.glsl\\'" . glsl-mode))
(add-to-list 'auto-mode-alist '("\\.vert\\'" . glsl-mode))
(add-to-list 'auto-mode-alist '("\\.frag\\'" . glsl-mode))
(add-to-list 'auto-mode-alist '("\\.geom\\'" . glsl-mode))

(add-hook 'find-file-hook
          (lambda ()
            (setq default-directory command-line-default-directory)))

(defun jump-to-mark()
  (interactive)
  (set-mark-command 1))

(defun build()
  (interactive)
  (compile "build.bat"))

(global-set-key "\M-s" 'save-buffer)
(global-set-key "\C-f" 'find-file)
(global-set-key "\M-r" 'revert-buffer)
(global-set-key "\M-l" 'kill-buffer)
(global-set-key (kbd "M-SPC") 'jump-to-mark)
(global-set-key "\M-f" 'imenu)
(global-set-key (kbd "C-/") 'query-replace)
(global-set-key (kbd "C-.") 'replace-string)
(global-set-key (kbd "C-,") 'fill-paragraph)
(global-set-key "\C-a" 'dabbrev-expand)
(global-set-key "\C-u" 'undo)
(global-set-key "\M-q" 'kill-region)
(global-set-key "\M-w" 'copy-region-as-kill)
(global-set-key "\M-e" 'yank)
(global-set-key [C-tab] 'other-window)
(global-set-key [M-left] 'switch-to-prev-buffer)
(global-set-key [M-right] 'switch-to-next-buffer)
(global-set-key [M-up] 'previous-error)
(global-set-key [M-down] 'next-error)
(global-set-key [f5] 'build)

;;switch between .cpp and .h files
(add-hook 'c-mode-common-hook
  (lambda() 
    (local-set-key "\M-c" 'ff-find-other-file)))

(add-hook 'c++-mode-hook
  (lambda() 
    (local-set-key "\M-c" 'ff-find-other-file)))

(setq fixme-modes '(c++-mode c-mode emacs-lisp-mode glsl-mode))
(make-face 'font-lock-todo-face)
(make-face 'font-lock-debug-face);
(mapc (lambda (mode)
        (font-lock-add-keywords
         mode
         '(("\\<\\(TODO\\)" 1 'font-lock-todo-face t)
	   ("\\<\\(DEBUG\\)" 1 'font-lock-debug-face t))))
      fixme-modes)
(modify-face 'font-lock-todo-face "Red" nil nil t nil t nil nil)
(modify-face 'font-lock-debug-face "Red" nil nil t nil t nil nil)

(defun c++-mode-indentation ()
  (setq c-basic-offset 4)
  (c-set-offset 'substatement-open 0))
(add-hook 'c++-mode-hook 'c++-mode-indentation)

(add-hook 'c++-mode-hook
          (lambda()
                  (local-set-key (kbd "RET") 'newline-and-indent)))

(add-hook 'c-mode-common-hook
          (lambda()
                  (local-set-key (kbd "RET") 'newline-and-indent)))

(setq undo-limit 20000000)
(setq undo-strong-limit 40000000)

(global-hl-line-mode 1)
(set-face-background 'hl-line "midnight blue")

(setq c-default-style "linux"
          c-basic-offset 4)

(global-hl-line-mode 1)
(setq-default indent-tabs-mode nil)
(delete-selection-mode 1)

(add-hook 'window-setup-hook (lambda () (split-window-horizontally)))
(defun maximize-frame ()
  "Maximizes the active frame in Windows"
  (interactive)
  (when (eq system-type 'windows-nt)
    (w32-send-sys-command 61488)))
(add-hook 'window-setup-hook 'maximize-frame t)

(scroll-bar-mode -1)
(tool-bar-mode -1)
;turning off the menu bar makes is so the frame won't fully maximize
;(menu-bar-mode -1)
;(defun menu-bar-off()
;  (menu-bar-mode -1))
;(add-hook 'window-setup-hook 'menu-bar-off)

(add-to-list 'default-frame-alist '(font . "Liberation Mono-10.0"))
(set-face-attribute 'default t :font  "Liberation Mono-10.0" )
(set-face-attribute 'font-lock-comment-face nil :foreground "light gray")
(set-face-attribute 'font-lock-string-face nil :foreground "olive drab")
(set-face-attribute 'font-lock-constant-face nil :foreground "olive drab")
(set-face-attribute 'font-lock-doc-face nil :foreground "dim gray")
(set-face-attribute 'font-lock-keyword-face nil :foreground "DarkGoldenrod3")
(set-face-attribute 'font-lock-builtin-face nil :foreground "#DAB98F")
(set-face-attribute 'font-lock-type-face nil :foreground "burlywood2")
(set-face-attribute 'font-lock-function-name-face nil :foreground "burlywood4")
(set-face-attribute 'font-lock-variable-name-face nil :foreground "burlywood4")
(add-to-list 'default-frame-alist '(foreground-color . "burlywood4"))
(add-to-list 'default-frame-alist '(background-color .  "#161616"))
(add-to-list 'default-frame-alist '(cursor-color . "#49FF40"))
(set-face-background 'hl-line "#242424")

(setq backup-directory-alist `(("." . "./backup")))
(setq backup-by-copying t)
(setq delete-old-versions t
  kept-new-versions 6
  kept-old-versions 2
  version-control t)

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(inhibit-startup-screen t)
 '(mouse-wheel-scroll-amount (quote (1 ((shift) . 1) ((control)))))
 '(show-paren-mode t))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )
(put 'downcase-region 'disabled nil)
(put 'upcase-region 'disabled nil)
