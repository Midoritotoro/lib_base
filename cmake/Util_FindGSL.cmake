macro(find_ms_gsl target_)
	set(Microsoft.GSL_DIR "D:/Libraries/win64/ms-gsl/share/Microsoft.GSL/")
	find_package(Microsoft.GSL CONFIG REQUIRED)
	target_link_libraries(${target_} PUBLIC Microsoft.GSL::GSL)
endmacro()