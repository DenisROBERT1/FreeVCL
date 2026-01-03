object Form_FileProperties: TForm_FileProperties
  Caption = 'Propriétés du fichier'
  Left = 296
  Top = 246
  Width = 436
  Height = 290
  Constraints.MinWidth = 436
  Constraints.MinHeight = 290
  Position = poMainFormCenter
  object Image_Icon: TImage
    Left = 8
    Top = 4
    Width = 48
    Height = 48
    TabStop = True
    TabOrder = 0
  end
  object Label_TypeProjet: TLabel
    Left = 60
    Top = 10
    Width = 370
    Height = 60
    Anchors = [akLeft, akTop, akRight]
    Alignment = taCenter
    AlignmentV = taTop
    Font.Height = -21
    Font.Style = [fsBold]
    Caption = ''
  end
  object Label_TextNom: TLabel
    Left = 8
    Top = 78
    Width = 80
    Height = 13
    Caption = 'Nom :'
    Alignment = taRightJustify
  end
  object Label_Nom: TLabel
    Left = 96
    Top = 78
    Width = 300
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    Caption = ''
  end
  object Label_TextFile: TLabel
    Left = 8
    Top = 98
    Width = 80
    Height = 13
    Caption = 'Fichier :'
    Alignment = taRightJustify
  end
  object Label_File: TLabel
    Left = 96
    Top = 98
    Width = 300
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    Caption = ''
  end
  object Label_TextLastModif: TLabel
    Left = 8
    Top = 118
    Width = 80
    Height = 13
    Caption = 'Modifié le :'
    Alignment = taRightJustify
  end
  object Label_LastModif: TLabel
    Left = 96
    Top = 118
    Width = 300
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    Caption = ''
  end
  object Label_TextBuilt: TLabel
    Left = 8
    Top = 138
    Width = 80
    Height = 13
    Caption = 'Construit le :'
    Alignment = taRightJustify
  end
  object Label_Built: TLabel
    Left = 96
    Top = 138
    Width = 300
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    Caption = ''
  end
  object Image_State: TImage
    Left = 66
    Top = 158
    Width = 20
    Height = 20
    TabStop = True
    TabOrder = 0
  end
  object Label_State: TLabel
    Left = 96
    Top = 158
    Width = 300
    Height = 60
    Anchors = [akLeft, akTop, akRight]
    Caption = ''
	WordWrap = True
  end
  object Button_Ok: TButton
    Left = 142
    Top = 228
    Width = 68
    Height = 22
    Caption = 'Ok'
    Anchors = [akLeft, akBottom]
    OnClick = Button_OkClick
    TabStop = True
    TabOrder = 0
    Default = True
	ModalResult = mrOk
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 230
    Top = 228
    Width = 68
    Height = 22
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 1
    OnClick = Button_AideClick
  end
end

