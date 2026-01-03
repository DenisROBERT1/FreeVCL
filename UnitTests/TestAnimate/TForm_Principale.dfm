object Form_Principale: TForm_Principale
  Left = 100
  Top = 100
  Width = 500
  Height = 420
  Constraints.MinWidth = 500
  Constraints.MinHeight = 400
  Caption = 'Animate'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  Position = 'poScreenCenter'
  object MainMenu: TMainMenu
    Left = 594
    object MenuItem_Fichiers: TMenuItem
      Caption = '&Fichiers'
      object MenuItem_Ouvrir: TMenuItem
        Caption = '&Ouvrir'
        ShortCut = 16463
        OnClick = MenuItem_OuvrirClick
      end
      object MenuItem_Play: TMenuItem
        Caption = '&Commencer l'#39'animation'
        ShortCut = 16464
        OnClick = MenuItem_PlayClick
      end
      object MenuItem_Stop: TMenuItem
        Caption = '&Stopper l'#39'animation'
        ShortCut = 16467
        OnClick = MenuItem_StopClick
      end
      object MenuItem_Separator1: TMenuItem
        Caption = '-'
      end
      object MenuItem_Quitter: TMenuItem
        Caption = '&Quitter'
        ShortCut = 16465
        OnClick = MenuItem_QuitterClick
      end
    end
  end
  object Animate: TAnimate
    Left = 8
    Top = 8
    Width = 476
    Height = 330
    Anchors = [akLeft, akTop, akRight, akBottom]
    Transparent = True
    CommonAVI = aviCopyFile
    Repetitions = 0
    StartFrame = 0
    StopFrame = -1
    Active = True
  end
  object CheckBox_Loop: TCheckBox
    Left = 8
    Top = 346
    Width = 100
    Height = 12
    Caption = 'Répétition'
    Anchors = [akLeft, akBottom]
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'avi'
    Filter = 'Fichiers AVI (*.avi)|*.avi|Tous (*.*)|*.*'
    Left = 504
    Top = 441
    Options = [ofHideReadOnly, ofFileMustExist]
  end
end
