object Form_FileFormat: TForm_FileFormat
  Caption = 'Format du fichier texte'
  Left = 442
  Top = 170
  Width = 367
  Height = 256
  Constraints.MinWidth = 367
  Constraints.MinHeight = 256
  Position = poOwnerFormCenter
  object CheckBox_ByteOrderMark;: TCheckBox
    Caption = 'En-tête de fichier (BOM)'
    Left = 12
    Top = 4
    Width = 340
    Height = 17
    TabOrder = 0
  end
  object CheckBox_BigEndian;: TCheckBox
    Caption = 'Octet de poids fort en premier (big endian)'
    Left = 12
    Top = 26
    Width = 340
    Height = 17
    TabOrder = 1
  end
  object GroupBox_FormatFichier: TGroupBox
    Caption = 'Format de caractères'
    Left = 8
    Top = 48
    Width = 344
    Height = 78
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 2
    object RadioButton_UTF8: TRadioButton
      Caption = 'UTF 8 (7 bits)'
      Left = 8
      Top = 20
      Width = 100
      Height = 14
      TabOrder = 0
    end
    object RadioButton_ASCII: TRadioButton
      Caption = 'ASCII (8 bits)'
      Left = 178
      Top = 20
      Width = 100
      Height = 16
      Anchors = [akLeft, akTop, akRight, akBottom]
      TabOrder = 1
    end
    object RadioButton_UNICODE: TRadioButton
      Caption = 'UNICODE (16 bits)'
      Left = 8
      Top = 48
      Width = 116
      Height = 14
      TabOrder = 2
    end
  end
  object GroupBox_FinLignes: TGroupBox
    Caption = 'Fin de lignes'
    Left = 8
    Top = 130
    Width = 344
    Height = 50
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 3
    object RadioButton_CRLF: TRadioButton
      Caption = 'Windows (CR LF)'
      Left = 8
      Top = 22
      Width = 112
      Height = 14
      Anchors = [akLeft, akTop, akRight, akBottom]
      TabOrder = 0
    end
    object RadioButton_LF: TRadioButton
      Caption = 'Linux (LF)'
      Left = 178
      Top = 22
      Width = 104
      Height = 14
      Anchors = [akLeft, akTop, akRight, akBottom]
      TabOrder = 1
    end
  end
  object Button_Ok: TButton
    Caption = 'Ok'
    Left = 100
    Top = 192
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    TabOrder = 4
		ModalResult = mrOk
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 190
    Top = 192
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    TabOrder = 5
		ModalResult = mrCancel
  end
end

