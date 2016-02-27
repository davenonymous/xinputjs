{
	"targets": [
		{
			"target_name": "xinput",
			"sources": [
				"src/XInputWrap.cc",
				"src/XInputFunctions.cc"
			],
			"include_dirs": [
				"<!(node -e \"require('nan')\")"
			],
			"libraries": [
				'-lXInput'
			]
		}
	]
}
