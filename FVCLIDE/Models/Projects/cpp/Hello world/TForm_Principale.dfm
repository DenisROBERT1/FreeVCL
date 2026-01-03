object Form_Principale: TForm_Principale
  Left = 100
  Top = 50
  Width = 300
  Height = 200
  Constraints.MinWidth = 300
  Constraints.MinHeight = 140
  Position = poScreenCenter
  Caption = 'Hello world'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  DesignSize = (
    866
    598)
  PixelsPerInch = 96
  TextHeight = 13
  object MainMenu: TMainMenu
    Left = 0
    Top = 0
    object MenuItem_Fichiers: TMenuItem
      Caption = 'Fichiers'
      object MenuItem_Quitter: TMenuItem
        Caption = 'Quitter'
        OnClick = MenuItem_QuitterClick
      end
    end
  end
  object Label_Hello: TLabel
    Left = 0
    Top = 20
    Width = 300
    Height = 20
    Alignment = taCenter
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Hello world !'
  end
  object Button_Ok: TButton
    Left = 110
    Top = 120
    Width = 80
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
    OnClick = Button_OkClick
    Default = False
  end
end
