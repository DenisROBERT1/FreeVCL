object Form_EdiOptions: TForm_EdiOptions
  Caption = 'Options générales de l'#39'éditeur'
  Left = 309
  Top = 149
  Width = 404
  Height = 408
  Constraints.MinWidth = 404
  Constraints.MinHeight = 408
  BorderIcons = [biSystemMenu]
	object Label_NbSpacesTab: TLabel
		Caption = 'Nombre d'#39'espaces des tabulations :'
		Left = 8
		Top = 6
		Width = 226
		Height = 14
	end
	object Edit_NbSpacesTab: TEdit
		Left = 256
		Top = 4
		Width = 48
		Height = 20
		Color = clWhite
		TabStop = True
		TabOrder = 0
		Text = '3'
	end
	object UpDown_NbSpacesTab: TUpDown
		Left = 304
		Top = 4
		Width = 16
		Height = 20
		Max = 20
		Increment = 1
		Position = 2
		Associate = Edit_NbSpacesTab
	end
	object CheckBox_SpacesToTabs: TCheckBox
		Caption = 'Transformation des espaces en tabulations'
		Left = 8
		Top = 32
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 1
		Alignment = taLeftJustify
		OnClick = CheckBox_SpacesToTabsClick
	end
	object CheckBox_TabsToSpaces: TCheckBox
		Caption = 'Transformation des tabulations en espaces'
		Left = 8
		Top = 54
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 2
		Alignment = taLeftJustify
		OnClick = CheckBox_TabsToSpacesClick
	end
	object CheckBox_LinesNumbers: TCheckBox
		Caption = 'Afficher les numéros de ligne'
		Left = 8
		Top = 76
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 3
		Alignment = taLeftJustify
	end
	object CheckBox_IconsColumn: TCheckBox
		Caption = 'Afficher les icônes d'#39'état'
		Left = 8
		Top = 98
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 4
		Alignment = taLeftJustify
	end
	object CheckBox_TypeAuto: TCheckBox
		Caption = 'Fermeture auto des parenthèses'
		Left = 8
		Top = 120
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 5
		Alignment = taLeftJustify
	end
	object CheckBox_IndentAuto: TCheckBox
		Caption = 'Indentation automatique'
		Left = 8
		Top = 142
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 6
		Alignment = taLeftJustify
	end
	object CheckBox_CurAfterEndLine: TCheckBox
		Caption = 'Curseur après la fin de ligne'
		Left = 8
		Top = 164
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 7
		Alignment = taLeftJustify
	end
	object CheckBox_UndoRedoMoveCur: TCheckBox
		Caption = 'Undo/redo des déplacements curseur'
		Left = 8
		Top = 186
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 8
		Alignment = taLeftJustify
	end
	object CheckBox_MultiLineTabs: TCheckBox
		Caption = 'Onglets sur plusieurs lignes'
		Left = 8
		Top = 208
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 9
		Alignment = taLeftJustify
	end
	object CheckBox_SaveBeforeBuild: TCheckBox
		Caption = 'Sauver les modifications avant compilation'
		Left = 8
		Top = 230
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 10
		Alignment = taLeftJustify
	end
	object CheckBox_Autocompletion: TCheckBox
		Caption = 'Activer l'#39'autocomplétion'
		Left = 8
		Top = 252
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 11
		Alignment = taLeftJustify
	end
	object CheckBox_AlphaBlend: TCheckBox
		Caption = 'Activer la transparence des fenêtres'
		Left = 8
		Top = 274
		Width = 262
		Height = 14
		TabStop = True
		TabOrder = 12
		Alignment = taLeftJustify
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
		TabOrder = 13
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
		TabOrder = 14
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
		TabOrder = 15
		OnClick = Button_AideClick
	end
end

