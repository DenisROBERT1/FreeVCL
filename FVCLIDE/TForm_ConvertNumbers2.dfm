object Form_ConvertNumbers: TForm_ConvertNumbers
  Caption = 'Transformation de formats'
  Left = 318
  Top = 274
  Width = 492
  Height = 238
  Constraints.MinWidth = 500
  Constraints.MinHeight = 249
  Constraints.MaxHeight = 249
  Font.Height = -11
  Font.Name = 'MS Shell Dlg'
  ActiveControl = TButton_Aide
  object TLabel_ChaineHexa: TLabel
    Caption = 'Chaîne hexa:'
    Left = 6
    Top = 6
    Width = 90
    Height = 13
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
  end
  object TEdit_ChaineHexa: TEdit
    Left = 102
    Top = 4
    Width = 250
    Height = 21
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
    TabOrder = 0
    ReadOnly = True
    SelStart = -1
    SelLength = 0
    OnChange = TEdit_ChaineHexaChange
  end
  object TLabel_ChaineASCII: TLabel
    Caption = 'Chaîne ASCII:'
    Left = 6
    Top = 32
    Width = 90
    Height = 13
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
  end
  object TEdit_ChaineAscii: TEdit
    Left = 102
    Top = 30
    Width = 250
    Height = 21
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
    TabOrder = 1
    ReadOnly = True
    SelStart = -1
    SelLength = 0
    OnChange = TEdit_ChaineAsciiChange
  end
  object TLabel_ChaineUNICODE: TLabel
    Caption = 'Chaîne UNICODE:'
    Left = 6
    Top = 58
    Width = 90
    Height = 13
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
  end
  object TEdit_ChaineUNICODE: TEdit
    Left = 102
    Top = 56
    Width = 250
    Height = 21
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
    TabOrder = 2
    ReadOnly = True
    SelStart = -1
    SelLength = 0
    OnChange = TEdit_ChaineAsciiChange
  end
  object TLabel_ShortInt: TLabel
    Caption = 'Entier 16 bits:'
    Left = 6
    Top = 84
    Width = 90
    Height = 13
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
  end
  object TEdit_ShortInt: TEdit
    Left = 102
    Top = 82
    Width = 250
    Height = 21
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
    TabOrder = 3
    ReadOnly = True
    SelStart = -1
    SelLength = 0
    OnChange = TEdit_ShortIntChange
  end
  object TLabel_LongInt: TLabel
    Caption = 'Entier 32 bits:'
    Left = 6
    Top = 110
    Width = 90
    Height = 13
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
  end
  object TEdit_LongInt: TEdit
    Left = 102
    Top = 108
    Width = 250
    Height = 21
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
    TabOrder = 4
    ReadOnly = True
    SelStart = -1
    SelLength = 0
    OnChange = TEdit_LongIntChange
  end
  object TLabel_Int64: TLabel
    Caption = 'Entier 64 bits:'
    Left = 6
    Top = 136
    Width = 90
    Height = 13
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
  end
  object TEdit_Int64: TEdit
    Left = 102
    Top = 134
    Width = 250
    Height = 21
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
    TabOrder = 5
    ReadOnly = True
    SelStart = -1
    SelLength = 0
    OnChange = TEdit_Int64Change
  end
  object TLabel_Float: TLabel
    Caption = 'Réel:'
    Left = 6
    Top = 162
    Width = 90
    Height = 13
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
  end
  object TEdit_Float: TEdit
    Left = 102
    Top = 160
    Width = 250
    Height = 21
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
    TabOrder = 6
    ReadOnly = True
    SelStart = -1
    SelLength = 0
    OnChange = TEdit_FloatChange
  end
  object TLabel_Double: TLabel
    Caption = 'Réel double:'
    Left = 6
    Top = 188
    Width = 90
    Height = 13
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
  end
  object TEdit_Double: TEdit
    Left = 102
    Top = 186
    Width = 250
    Height = 22
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
    TabOrder = 7
    ReadOnly = True
    SelStart = -1
    SelLength = 0
    OnChange = TEdit_DoubleChange
  end
  object TButton_RotLeft: TButton
    Caption = '<<'
    Left = 358
    Top = 3
    Width = 30
    Height = 22
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akTop, akRight]
    ShowHint = True
    Hint = 'Rotation à gauche'
    OnClick = TButton_RotLeftClick
    TabStop = True
    TabOrder = 8
  end
  object TButton_RotRight: TButton
    Caption = '>>'
    Left = 394
    Top = 3
    Width = 30
    Height = 22
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akTop, akRight]
    ShowHint = True
    Hint = 'Rotation à droite'
    OnClick = TButton_RotRightClick
    TabStop = True
    TabOrder = 9
  end
  object TButton_Invert: TButton
    Caption = 'Sens'
    Left = 430
    Top = 3
    Width = 48
    Height = 22
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akTop, akRight]
    ShowHint = True
    Hint = 'Inversion du sens des octets'
    OnClick = TButton_InvertClick
    TabStop = True
    TabOrder = 10
  end
  object TCheckBox_Signe: TCheckBox
    Caption = 'Signé'
    Left = 376
    Top = 109
    Width = 88
    Height = 16
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akTop, akRight]
    OnClick = TCheckBox_SigneClick
    TabStop = True
    TabOrder = 11
  end
  object TShape1: TShape
    Caption = 'TShape'
    Left = 360
    Top = 82
    Width = 8
    Height = 72
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clWhite
    Pen.Color = clNone
    Brush = 0
  end
  object TButton_Aide: TButton
    Caption = 'Aide'
    Left = 420
    Top = 184
    Width = 56
    Height = 20
    Font.Height = -11
    Font.Name = 'MS Shell Dlg'
    Anchors = [akRight, akBottom]
    TabStop = True
  end
end

