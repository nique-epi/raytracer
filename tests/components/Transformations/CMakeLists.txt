raytracer_add_test(
	NAME rotation_transformation_test
	SOURCES RotationTransformationTests.cpp
	LIBS raytracer_rotation raytracer_math
)

raytracer_add_test(
	NAME translation_transformation_test
	SOURCES TranslationTransformationTests.cpp
	LIBS raytracer_translation raytracer_math
)

raytracer_add_test(
	NAME scale_transformation_test
	SOURCES ScaleTransformationTests.cpp
	LIBS raytracer_scale raytracer_math
)
