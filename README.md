# Health Check System (C)

A console program that collects basic health inputs from the user and
evaluates them against defined thresholds to flag risk indicators.

## Inputs

* Age, gender
* Height (cm) and weight (kg) — used to compute BMI
* Systolic / diastolic blood pressure
* Resting heart rate
* Body temperature (C)
* Fasting blood sugar (mg/dL)

## Output

* BMI category
* Blood pressure category
* Heart rate category
* Temperature category
* Blood sugar category
* Overall risk level (Low / Moderate / High) based on number of flagged indicators

## Build & Run

```bash
make          # build health_check
make run      # build and run
make clean    # remove build artifacts
```

## Requirements

* A C11 compiler (`gcc` recommended)
* GNU Make

## Disclaimer

This is a basic screening tool, not a medical diagnosis. Consult a
healthcare professional for proper evaluation.

## License

MIT