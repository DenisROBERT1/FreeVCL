object Form_ConvertNumbers: TForm_ConvertNumbers
  Caption = 'Transformation de formats'
  Left = 318
  Top = 277
  Width = 492
  Height = 238
  Position = poOwnerFormCenter
  Constraints.MinWidth = 492
  Constraints.MinHeight = 238
  Constraints.MaxHeight = 238
  object Label_ChaineHexa: TLabel
    Caption = 'Chaîne hexa:'
    Left = 6
    Top = 6
    Width = 90
    Height = 13
  end
  object Edit_ChaineHexa: TEdit
    Left = 102
    Top = 4
    Width = 250
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
		TabOrder = 0
		OnChange = Edit_ChaineHexaChange
  end
  object Label_ChaineASCII: TLabel
    Caption = 'Chaîne ASCII:'
    Left = 6
    Top = 32
    Width = 90
    Height = 13
  end
  object Edit_ChaineAscii: TEdit
    Left = 102
    Top = 30
    Width = 250
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
		TabOrder = 1
		OnChange = Edit_ChaineAsciiChange
  end
  object Label_ChaineUnicode: TLabel
    Caption = 'Chaîne UNICODE:'
    Left = 6
    Top = 58
    Width = 90
    Height = 13
  end
  object Edit_ChaineUnicode: TEdit
    Left = 102
    Top = 56
    Width = 250
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
		TabOrder = 2
		OnChange = Edit_ChaineUnicodeChange
  end
  object Label_ShortInt: TLabel
    Caption = 'Entier 16 bits:'
    Left = 6
    Top = 84
    Width = 90
    Height = 13
  end
  object Edit_ShortInt: TEdit
    Left = 102
    Top = 82
    Width = 250
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    TabStop = True
		TabOrder = 3
		OnChange = Edit_ShortIntChange
  end
  object Label_LongInt: TLabel
    Caption = 'Entier 32 bits:'
    Left = 6
    Top = 110
    Width = 90
    Height = 13
  end
  object Edit_LongInt: TEdit
    Left = 102
    Top = 108
    Width = 250
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
		TabOrder = 4
		OnChange = Edit_LongIntChange
  end
  object Label_Int64: TLabel
    Caption = 'Entier 64 bits:'
    Left = 6
    Top = 136
    Width = 90
    Height = 13
  end
  object Edit_Int64: TEdit
    Left = 102
    Top = 134
    Width = 250
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
		TabOrder = 5
		OnChange = Edit_Int64Change
  end
  object Label_Float: TLabel
    Caption = 'Réel:'
    Left = 6
    Top = 162
    Width = 90
    Height = 13
  end
  object Edit_Float: TEdit
    Left = 102
    Top = 160
    Width = 250
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
		TabOrder = 6
		OnChange = Edit_FloatChange
  end
  object Label_Double: TLabel
    Caption = 'Réel double:'
    Left = 6
    Top = 188
    Width = 90
    Height = 13
  end
  object Edit_Double: TEdit
    Left = 102
    Top = 186
    Width = 250
    Height = 22
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
		TabOrder = 7
		OnChange = Edit_DoubleChange
  end
  object Button_RotLeft: TButton
    Caption = '<<'
    Left = 358
    Top = 3
    Width = 30
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
		TabOrder = 8
		ShowHint = True
		Hint = 'Rotation à gauche'
		OnClick = Button_RotLeftClick
  end
  object Button_RotRight: TButton
    Caption = '>>'
    Left = 394
    Top = 3
    Width = 30
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
		TabOrder = 9
		ShowHint = True
		Hint = 'Rotation à droite'
		OnClick = Button_RotRightClick
  end
  object Button_Invert: TButton
    Caption = 'Sens'
    Left = 430
    Top = 3
    Width = 48
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
		TabOrder = 10
		ShowHint = True
		Hint = 'Inversion du sens des octets'
		OnClick = Button_InvertClick
  end
  object Shape_1: TShape
    Left = 360
    Top = 78
    Width = 8
    Height = 80
    Anchors = [akTop, akRight]
		object Shape_2: TShape
			Left = 0
			Top = 4
			Width = 4
			Height = 72
			Anchors = [akTop, akRight]
		end
  end
  object CheckBox_Signe: TCheckBox
    Caption = 'Signé'
    Left = 376
    Top = 109
    Width = 88
    Height = 16
    Anchors = [akTop, akRight]
    TabStop = True
		TabOrder = 11
		OnClick = CheckBox_SigneClick
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 420
    Top = 184
    Width = 56
    Height = 20
    Anchors = [akRight, akBottom]
    TabStop = True
		OnClick = Button_AideClick
  end
end

