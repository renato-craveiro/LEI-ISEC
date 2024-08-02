package pt.isec.a2018011392;

public enum SafeState {
    OPEN,
    CLOSED,
    LOCKED;

    ISafeState getState(SafeContext context, SafeData data) {
        return switch (this) {
            case OPEN -> new OpenState(context, data);
            case CLOSED -> new ClosedState(context, data);
            case LOCKED -> new LockedState(context, data);
            default -> null;
        };
    }
}
