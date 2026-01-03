object FormPrincipale: TFormPrincipale
  Left = 390
  Top = 252
  Width = 242
  Height = 144
  Caption = 'FormPrincipale'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 6
    Top = 12
    Width = 85
    Height = 34
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 6
    Top = 63
    Width = 85
    Height = 34
    Caption = 'Button2'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 102
    Top = 12
    Width = 85
    Height = 34
    Caption = 'Button3'
    TabOrder = 2
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 102
    Top = 63
    Width = 85
    Height = 34
    Caption = 'Button4'
    TabOrder = 3
    OnClick = Button4Click
  end
  object PrintDialog1: TPrintDialog
    Left = 192
    Top = 15
  end
  object PrinterSetupDialog1: TPrinterSetupDialog
    Left = 192
    Top = 66
  end
end
