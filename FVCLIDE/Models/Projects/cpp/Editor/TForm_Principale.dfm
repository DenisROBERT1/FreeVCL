object Form_Principale: TForm_Principale
  Caption = 'Exemple editeur'
  Left = 100
  Top = 50
  Width = 678
  Height = 501
  Constraints.MinWidth = 678
  Constraints.MinHeight = 501
  Menu = MainMenu
  object MainMenu: TMainMenu
    object MenuItem_Fichiers: TMenuItem
      Caption = '&Fichiers'
      object MenuItem_New: TMenuItem
        Caption = '&Nouveau'
        ShortCut = '16462'
        OnClick = MenuItem_NewClick
      end
      object MenuItem_Open: TMenuItem
        Caption = '&Ouvrir'
        ShortCut = '16463'
        OnClick = MenuItem_OpenClick
      end
      object MenuItem_Save: TMenuItem
        Caption = '&Enregistrer'
        ShortCut = '16467'
        OnClick = MenuItem_SaveClick
      end
      object MenuItem_SaveAs: TMenuItem
        Caption = 'En&registrer sous'
        OnClick = MenuItem_SaveAsClick
      end
      object MenuItem_Separator1: TMenuItem
        Caption = '-'
      end
      object MenuItem_Print: TMenuItem
        Caption = '&Imprimer'
        ShortCut = '16464'
        OnClick = MenuItem_PrintClick
      end
      object MenuItem_Separator2: TMenuItem
        Caption = '-'
      end
      object MenuItem_Quit: TMenuItem
        Caption = '&Quitter'
        ShortCut = '16465'
        OnClick = MenuItem_QuitClick
      end
    end
    object MenuItem_Help: TMenuItem
      Caption = '&Aide'
      RightJustify = True
      object MenuItem_OnlineHelp: TMenuItem
        Caption = '&Aide'
        ShortCut = '112'
        OnClick = MenuItem_OnlineHelpClick
      end
      object MenuItem_About: TMenuItem
        Caption = 'A &propos'
        OnClick = MenuItem_AboutClick
      end
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 434
    Width = 670
    Height = 20
    Align = alBottom
    Panels = 0
    PanelText.Strings = (
      ''
    )
  end
  object Memo_Text: TMemo
    Left = 4
    Top = 4
    Width = 660
    Height = 426
    Anchors = [akLeft, akTop, akRight, akBottom]
    ScrollBars = ssVertical
    OnChange = Memo_TextChange
  end
  object OpenDialog: TOpenDialog
    Filter = 'Tous (*.*)|*.*'
    Options = [ofHideReadOnly, ofFileMustExist]
  end
  object SaveDialog: TSaveDialog
    Filter = 'Tous (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly]
  end
  object PrintDialog: TPrintDialog
  end
end

