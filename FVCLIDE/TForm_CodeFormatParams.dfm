object Form_CodeFormatParams: TForm_CodeFormatParams
  Caption = 'Paramètres de formatage de code'
  Left = 309
  Top = 149
  Width = 404
  Height = 408
  Constraints.MinWidth = 404
  Constraints.MinHeight = 408
  BorderIcons = [biSystemMenu]
	object CheckBox_EndLineAcc: TCheckBox
		Caption = 'Accolades en fin de ligne'
		Left = 8
		Top = 6
		Width = 246
		Height = 14
		TabStop = True
		TabOrder = 0
		Alignment = taLeftJustify
		OnClick = CheckBox_EndLineAccClick
	end
	object CheckBox_SpacesAfterPar: TCheckBox
		Caption = 'Espace après parenthèses'
		Left = 8
		Top = 28
		Width = 246
		Height = 14
		TabStop = True
		TabOrder = 1
		Alignment = taLeftJustify
		OnClick = CheckBox_SpacesAfterParClick
	end
	object Label_Apercu: TLabel
		Caption = 'Aperçu :'
		Left = 8
		Top = 50
		Width = 246
		Height = 14
		TabStop = True
	end
	object Label_Langage: TLabel
		Caption = 'Langage :'
		Left = 250
		Top = 50
		Width = 68
		Height = 14
		Anchors = [akTop, akRight]
	end
	object ComboBox_Langage: TComboBox
		Left = 308
		Top = 48
		Width = 80
		Height = 22
		Anchors = [akTop, akRight]
		TabStop = True
		TabOrder = 6
		Items.Strings = (
			'cpp'
			'pas'
			'dfm'
			'html')
		ItemIndex = 0;
		OnChange = ComboBox_LangageChange
	end
	object Frame_EditFichierText: TFrame_EditFichierText
		Left = 8
		Top = 72
		Width = 380
		Height = 240
		TabStop = True
		TabOrder = 2
		Anchors = [akLeft, akTop, akRight, akBottom]
	end
  object Button_Ok: TButton
    Caption = 'Ok'
    Left = 58
    Top = 336
    Width = 80
    Height = 25
    Anchors = [akLeft, akBottom]
    OnClick = Button_OkClick
    TabStop = True
    TabOrder = 4
    Default = True
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 158
    Top = 336
    Width = 80
    Height = 25
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 5
    Cancel = True
    ModalResult = mrCancel
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 258
    Top = 336
    Width = 80
    Height = 25
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 6
    OnClick = Button_AideClick
  end
end

