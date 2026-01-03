object Form_Autocompletion: TForm_Autocompletion
	Left = 0
	Top = 0
	Width = 104
	Height = 68
	Visible = False
	Caption = '[AUTOCOMP]'
	FormActivation = False
	FormStyle = fsStayOnTop
	AlphaBlend = True
	AlphaBlendValue = 31
	BorderIcons = [biSystemMenu]
	BorderStyle = bsNone
	object Panel_Autocompletion: TPanel
		Align = alClient
		BorderStyle = bsNone
		BorderWidth = 3
		BevelWidth = 1
		BevelOuter = bvRaised
		BevelInner = bvLowered
		object PaintBox_Autocompletion: TPaintBox
			Align = alClient
			OnPaint = PaintBox_AutocompletionPaint
			OnMouseMove = PaintBox_AutocompletionMouseMove
			OnMouseWheel = PaintBox_AutocompletionMouseWheel
			OnMouseDown = PaintBox_AutocompletionMouseDown
		end
	end
	object Timer_AlphaBlend: TTimer
		Interval = 50
		Enabled = True
		OnTimer = Timer_AlphaBlendTimer
	end
end
