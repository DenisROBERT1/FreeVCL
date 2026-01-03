object Form_Principale: TForm_Principale
  Left = 100
  Top = 100
  Width = 500
  Height = 400
  Constraints.MinWidth = 500
  Constraints.MinHeight = 400
  Caption = 'WebBrowser'
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
  object Label_Adresse: TLabel
    Left = 8
    Top = 8
    Width = 80
    Height = 13
    Caption = 'Adresse WEB'
  end
  object Edit_Adresse: TEdit
    Left = 92
    Top = 6
    Width = 300
    Height = 21
    TabOrder = 0
    Text = 'http://denis.beru.free.fr'
  end
  object Button_Go: TButton
    Left = 400
    Top = 6
    Width = 84
    Height = 22
    Caption = 'Go'
    TabOrder = 1
    OnClick = Button_GoClick
  end
  object CppWebBrowser: TCppWebBrowser
    Left = 8
    Top = 36
    Width = 476
    Height = 330
    Anchors = [akLeft, akTop, akRight, akBottom]
  end
end
