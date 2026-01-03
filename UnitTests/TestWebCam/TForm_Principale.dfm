object Form_Principale: TForm_Principale
  Left = 100
  Top = 100
  Width = 500
  Height = 400
  Constraints.MinWidth = 500
  Constraints.MinHeight = 400
  Caption = 'WebCam'
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
  object WebCam: TWebCam
    Left = 8
    Top = 8
    Width = 476
    Height = 330
    Anchors = [akLeft, akTop, akRight, akBottom]
    PreviewRate = 60
    PreviewScale = True
    Preview = True
    OnFrame = WebCamFrame
  end
  object Button_Param: TButton
    Left = 8
    Top = 342
    Width = 84
    Height = 22
    Anchors = [akLeft, akBottom]
    Caption = 'Paramètres'
    TabOrder = 1
    OnClick = Button_ParamClick
  end
  object Button_Capture: TButton
    Left = 112
    Top = 342
    Width = 84
    Height = 22
    Anchors = [akLeft, akBottom]
    Caption = 'Capture'
    TabOrder = 1
    OnClick = Button_CaptureClick
  end
  object SaveDialog: TSaveDialog
    Filter = 'Bitmaps Windows (*.bmp)|*.bmp|Tous (*.*)|*.*'
    DefaultExt = 'bmp'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 498
    Top = 354
  end
end
