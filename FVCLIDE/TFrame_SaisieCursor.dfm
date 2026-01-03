object Frame_SaisieCursor: TFrame_SaisieCursor
  Left = 282
  Top = 133
  Width = 460
  Height = 383
  Color = clBtnFace
  Visible = True
  OnShow = Frame_SaisieCursorShow
  object Panel: TPanel
		Left = 6
		Top = 6
		Width = 439
		Height = 325
		Align = alClient
		TabStop = True
		TabOrder = 0
		BevelOuter = bvLowered
		object Button_AjouterImage: TButton
			Left = 4
			Top = 8
			Width = 96
			Height = 21
			Anchors = [akLeft, akTop]
			Caption = 'Ajouter une image'
			OnClick = Button_AjouterImageClick
		end
		object Button_EnregistrerImageSous: TButton
			Left = 104
			Top = 8
			Width = 96
			Height = 21
			Anchors = [akLeft, akTop]
			Caption = 'Enregistrer l'#39'image'
			OnClick = Button_EnregistrerImageSousClick
		end
		object Button_EditerImage: TButton
			Left = 204
			Top = 8
			Width = 96
			Height = 21
			Anchors = [akLeft, akTop]
			Caption = 'Editer l'#39'image'
			OnClick = Button_EditerImageClick
		end
		object Button_EnleverImage: TButton
			Left = 304
			Top = 8
			Width = 96
			Height = 21
			Anchors = [akLeft, akTop]
			Caption = 'Enlever l'#39'image'
			OnClick = Button_EnleverImageClick
		end
		object Button_MiseAZero: TButton
			Left = 404
			Top = 8
			Width = 96
			Height = 21
			Anchors = [akLeft, akTop]
			Caption = 'Mise à zéro'
			OnClick = Button_MiseAZeroClick
		end
		object Button_Aide: TButton
			Left = 504
			Top = 8
			Width = 96
			Height = 21
			Anchors = [akLeft, akTop]
			Caption = 'Aide'
			OnClick = Button_AideClick
		end
		object TabControl: TTabControl
			Left = 0
			Top = 50
			Width = 492
			Height = 356
			Anchors = [akLeft, akTop, akRight, akBottom]
			TabOrder = 0
			OnChange = TabControlChange
			DesignSize = (
				492
				356)
			object PaintBox: TPaintBox
				Left = 6
				Top = 27
				Width = 320
				Height = 320
				Anchors = [akLeft, akTop]
				OnPaint = PaintBoxPaint
				OnMouseMove = PaintBoxMouseMove
			end
			object Label_Masque: TLabel
				Left = 347
				Top = 27
				Width = 50
				Height = 13
				Anchors = [akLeft, akTop]
				Caption = 'Masque :'
			end
			object PaintBoxMask: TPaintBox
				Left = 347
				Top = 48
				Width = 133
				Height = 133
				Anchors = [akLeft, akTop]
				OnPaint = PaintBoxMaskPaint
				OnMouseMove = PaintBoxMaskMouseMove
			end
			object Label_XHotSpot: TLabel
				Left = 347
				Top = 192
				Width = 50
				Height = 21
				Anchors = [akLeft, akTop]
				Caption = 'X hotspot'
			end
			object Edit_XHotSpot: TEdit
				Left = 400
				Top = 190
				Width = 50
				Height = 21
				Anchors = [akLeft, akTop]
				OnChange = Edit_XHotSpotChange
			end
			object UpDown_XHotSpot: TUpDown
				Left = 450
				Top = 190
				Width = 19
				Height = 21
				Anchors = [akLeft, akTop]
        Position = 0
        Associate = Edit_XHotSpot
			end
			object Label_YHotSpot: TLabel
				Left = 347
				Top = 218
				Width = 50
				Height = 21
				Anchors = [akLeft, akTop]
				Caption = 'Y hotspot'
			end
			object Edit_YHotSpot: TEdit
				Left = 400
				Top = 216
				Width = 50
				Height = 21
				Anchors = [akLeft, akTop]
				OnChange = Edit_YHotSpotChange
			end
			object UpDown_YHotSpot: TUpDown
				Left = 450
				Top = 216
				Width = 19
				Height = 21
				Anchors = [akLeft, akTop]
        Position = 0
        Associate = Edit_YHotSpot
			end
		end
  end
  object OpenDialogBmp: TOpenDialog
    Filter = 'Bitmaps Windows|*.bmp|Tous|*.*'
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 468
    Top = 354
  end
  object SaveDialogBmp: TSaveDialog
    Filter = 'Bitmaps Windows|*.bmp|Tous|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 498
    Top = 354
  end
end
