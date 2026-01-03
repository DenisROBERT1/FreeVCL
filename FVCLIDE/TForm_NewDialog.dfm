object Form_NewDialog: TForm_NewDialog
  Caption = 'Nouveau dialogue'
  Left = 381
  Top = 202
  Width = 500
  Height = 476
  Position = poMainFormCenter
  Constraints.MinWidth = 500
  Constraints.MinHeight = 420
  object Label_Unite: TLabel
    Left = 6
    Top = 6
    Width = 70
    Height = 13
    Caption = 'Nom unité :'
  end
  object Label_T: TLabel
    Left = 76
    Top = 6
    Width = 6
    Height = 13
    Alignment = taRightJustify
    Caption = 'T'
  end
  object Edit_Unite: TEdit
    Left = 84
    Top = 4
    Width = 144
    Height = 21
    AutoSize = False
    TabOrder = 2
    Anchors = [akLeft, akTop, akRight]
		Text = 'Form_'
  end
  object Label_Commentaire: TLabel
    Left = 232
    Top = 6
    Width = 200
    Height = 13
    Anchors = [akTop, akRight]
    Caption = '(Nom de la classe = T + nom unité)'
  end
  object Label_Caption: TLabel
    Left = 6
    Top = 32
    Width = 70
    Height = 13
    Caption = 'Titre fenêtre :'
  end
  object Edit_Caption: TEdit
    Left = 84
    Top = 30
    Width = 404
    Height = 21
    TabOrder = 3
    Anchors = [akLeft, akTop, akRight]
  end
  object GroupBox_Options: TGroupBox
    Caption = 'Type de dialogue à créer'
    Left = 4
    Top = 58
    Width = 484
    Height = 108
    Anchors = [akLeft, akTop, akRight]
    object RadioButton_Empty: TRadioButton
      Caption = 'Boîte de dialogue vide'
      Left = 8
      Top = 18
      Width = 472
      Height = 16
	    Anchors = [akLeft, akTop, akRight]
      TabStop = True
      Checked = True
	    OnClick = RadioButton_EmptyClick
    end
    object RadioButton_OkCancelHelp: TRadioButton
      Caption = 'Boutons "Ok", "Annuler" et "Aide"'
      Left = 8
      Top = 38
      Width = 472
      Height = 16
	    Anchors = [akLeft, akTop, akRight]
      TabStop = True
	    OnClick = RadioButton_OkCancelHelpClick
    end
    object RadioButton_Structure: TRadioButton
      Caption = 'À partir d'#39'une structure'
      Left = 8
      Top = 58
      Width = 472
      Height = 16
	    Anchors = [akLeft, akTop, akRight]
      TabStop = True
	    OnClick = RadioButton_StructureClick
    end
    object RadioButton_FromWindow: TRadioButton
      Caption = 'À partir d'#39'une fenêtre existante'
      Left = 8
      Top = 78
      Width = 472
      Height = 16
	    Anchors = [akLeft, akTop, akRight]
      TabStop = True
	    OnClick = RadioButton_FromWindowClick
    end
  end
  object GroupBox_Informations: TGroupBox
    Caption = 'Informations complémentaires'
    Left = 4
    Top = 172
    Width = 484
    Height = 216
    Anchors = [akLeft, akTop, akRight, akBottom]
		object Label_Struct: TLabel
			Left = 8
			Top = 16
			Width = 468
			Height = 44
	    Anchors = [akLeft, akTop, akRight]
			Caption = 'Structure à saisir. Par exemple:'#10'char Nom[128]; // Nom'#10'char Prenom[128]; // Prénom'
			Visible = False
		end
		object Memo_Struct: TMemo
			Left = 8
			Top = 62
			Width = 468
			Height = 146
			Anchors = [akLeft, akTop, akRight, akBottom]
			Font.Charset = DEFAULT_CHARSET
			Font.Color = clWindowText
			Font.Height = -11
			Font.Name = 'Courier New'
			Font.Style = []
			ParentFont = False
			TabOrder = 1
			Visible = False
		end
    object Label_Windows: TLabel
	    Caption = 'Liste des fenêtres existantes :'
      Left = 8
      Top = 16
      Width = 468
      Height = 12
	    Anchors = [akLeft, akTop, akRight]
			Visible = False
    end
    object ListBox_Windows: TListBox
      Left = 8
      Top = 32
      Width = 468
      Height = 176
	    Anchors = [akLeft, akTop, akRight]
      TabStop = True
			TabOrder = 0
			Visible = False
    end
  end
  object Button_Ok: TButton
    Caption = 'Ok'
    Left = 72
    Top = 410
    Width = 72
    Height = 24
    Anchors = [akLeft, akBottom]
    TabOrder = 3
    Default = True
    OnClick = Button_OkClick
  end
  object Button_Apercu: TButton
    Caption = 'Aperçu'
    Left = 164
    Top = 410
    Width = 72
    Height = 24
    Anchors = [akLeft, akBottom]
    TabOrder = 2
    OnClick = Button_ApercuClick
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 256
    Top = 410
    Width = 72
    Height = 24
    Anchors = [akLeft, akBottom]
    TabOrder = 4
    Cancel = True
    ModalResult = mrCancel
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 348
    Top = 410
    Width = 72
    Height = 24
    Anchors = [akLeft, akBottom]
    TabOrder = 5
    OnClick = Button_AideClick
  end
end

